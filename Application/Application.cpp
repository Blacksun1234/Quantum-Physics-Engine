// Physics-engine.cpp�: d�finit le point d'entr�e pour l'application console.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <ctime>
#include <string>

#include "Quantum.h"
#include "GxWorld.h"
#include "GxParticle.h"
#include "GxUpdater.h"
#include "QmForceRegistery.h"
#include "QmDrag.h"
#include "QmMagnetism.h"

using namespace std;
using namespace Quantum;


GxWorld gxWorld;
QmWorld pxWorld;

bool Boolean_garvity = true;

glm::vec3* mousePointer;

int scene = 0;

// ********************** GLUT 
// Variables globales

int SCREEN_X = 800;
int SCREEN_Y = 800;
int VIEWPORT_X = 5; int VIEWPORT_Y = 5; int VIEWPORT_Z = 200;

GLuint DrawListSphere;

int buttons = 0;
float camRotate = 90.f;
float camDist = 30.f;
float camHeight = 0.f;
float mx = 0.f;
float my = 0.f;

// Variables du calcul de framerate 
int timeold = 0;
int frame = 0;
float accTime = 0.0;
bool paused = false;

GLfloat light_pos[] = { 0.0, 6.0, 1.0, 1.0 };

// ********************************************


glm::vec3 randomVector(float min, float max)
{
	float x = min + (max - min)*((rand() % 100) / 100.f);
	float y = min + (max - min)*((rand() % 100) / 100.f);
	float z = min + (max - min)*((rand() % 100) / 100.f);
	return glm::vec3(x, y, z);

}

QmParticle* createParticle()
{
	for (int i = 0; i < 50; i++) {

		glm::vec3 pos = randomVector(-5, 5);
		GxParticle* g = new GxParticle(randomVector(1, 0), 0.1f + 0.2f*((rand() % 100) / 100.f), pos);
		QmParticle* p = new QmParticle(pos, randomVector(0, 0), randomVector(-1, 1),3);
		p->setUpdater(new GxUpdater(g));
		gxWorld.addParticle(g);
		pxWorld.addBody(p);

		//add particules and link them in the forceRegistery in both ways

		for (QmBody* b : pxWorld.bodies) {
			if (b != p) {
				QmForceRegistery* f = new QmForceRegistery(p, new QmMagnetism(1, 2));
				pxWorld.addForceRegistery(f);
			}
		}

		return p;
	}
	
}

QmParticle* createParticleFromCursor()
{
	glm::vec3 pos = *mousePointer;
	GxParticle* g = new GxParticle(randomVector(1, 0), 0.1f + 0.2f * ((rand() % 100) / 100.f), pos);
	QmParticle* p = new QmParticle(pos, randomVector(1, 10), randomVector(0, 0), 3);
	QmForceRegistery* f = new QmForceRegistery(p, new QmDrag(1,2));
	p->setUpdater(new GxUpdater(g));
	gxWorld.addParticle(g);
	pxWorld.addBody(p);
	pxWorld.addForceRegistery(f);
	
	return p;
}

void initScene1()
{
	printf("Scene 1: Random particles.\n");
	printf("Type space to pause.\n");
	mousePointer = new glm::vec3(0, 4.5, 0);
}

void initScene2()
{
	printf("Scene 2.\n");
	mousePointer = new glm::vec3(0, 4.5, 0);
	// wrigth function to create some partcules (50) and link them in the forceRegistery in both ways
}

// ***************************** GLUT methods

void resetView()
{
	camRotate = 90.f;
	camDist = 30.f;
	camHeight = 0.f;
	mx = 0.f;
	my = 0.f;
	if (mousePointer) {
		delete mousePointer;
		mousePointer = NULL;
	}
}

void createDisplayListSphere()
{
	DrawListSphere = glGenLists(1);
	glNewList(DrawListSphere, GL_COMPILE);
	GLUquadricObj* sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, 1, 12, 8);
	glEndList();
}

void initLight()
{
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);

	GLfloat ambient_light[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuse_light[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
}

void calculateFPS(float dt)
{
	accTime += dt;

	if (accTime > 0.5)
	{
		char t[200];
		sprintf_s(t, "Quantum Physics Engine: Scene %d, %.2f FPS", scene, frame / accTime);
		glutSetWindowTitle(t);
		accTime = 0.0;
		frame = 0;
	}
}

void idleFunc()
{
	int timer = glutGet(GLUT_ELAPSED_TIME);
	float dt = (float)(timer - timeold) / 1000.f;
	timeold = timer;

	calculateFPS(dt);
	if (!paused) pxWorld.simulate(dt);

	if (buttons == 3) {
			createParticleFromCursor();
	}

	glutPostRedisplay();
}

void drawFunc()
{
	frame++;
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camDist * cos(camRotate), camHeight, camDist * sin(camRotate), 0, camHeight, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	for (GxParticle* p : gxWorld.getParticles())
	{
		glPushMatrix();
		glm::vec3 color = p->getColor();
		glColor3f(color.x, color.y, color.z);
		glTranslatef(p->getPos().x, p->getPos().y, p->getPos().z);
		glScaled(p->getRadius(), p->getRadius(), p->getRadius());
		glCallList(DrawListSphere);
		glPopMatrix();
	}
	if (mousePointer) {
		glPushMatrix();
		glColor3f(1.f, 1.f, 1.f);
		glTranslatef(mousePointer->x, mousePointer->y, mousePointer->z);
		glScaled(0.1f, 0.1f, 0.1f);
		glCallList(DrawListSphere);
		glPopMatrix();
	}
	glutSwapBuffers();
}


void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) buttons = 1;
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) buttons = 2;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) buttons = 3;
	if (button == 3) camDist /= 1.07f;
	if (button == 4) camDist *= 1.07f;
	if (state == GLUT_UP) buttons = 0;

	mx = (float)x;
	my = (float)y;
}

void motionFunc(int x, int y)
{
	if (buttons == 1)
	{
		camRotate += (x - mx) / 100.f;
		camHeight += (y - my) * camDist / 1000.f;
	}
	if (buttons == 3)
	{
		if (mousePointer)
			*mousePointer += glm::vec3(x - mx, my - y, 0.f) / 15.f;
	}

	mx = (float)x;
	my = (float)y;
}

void clearWorld()
{
	gxWorld.clear();
	pxWorld.clear();

}

void toggleScene(int s)
{
	clearWorld();
	scene = s;
	resetView();

	switch (scene)
	{
	case 1: initScene1(); break;
	case 2: initScene2(); break;
	}
}


void keyFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q': case 'Q': case 27:
		clearWorld();
		glDeleteLists(DrawListSphere, 1);
		exit(0);
		break;
	case '1':
		Boolean_garvity = true;
		toggleScene(1);
		break;
	case '2':
		Boolean_garvity = false;
		toggleScene(2);
		break;
	case ' ':
		paused = !paused;
		break;
	case 'g':
		cout << Boolean_garvity << endl;
		if (Boolean_garvity) {
			Boolean_garvity = false;
			pxWorld.SetGravity(Boolean_garvity);
			cout << "Gravity off " << Boolean_garvity << endl;
		}
		else {
			Boolean_garvity = true;
			cout << "Gravity on " << Boolean_garvity << endl;
			pxWorld.SetGravity(Boolean_garvity);
		}
	default:
		break;
	}
}

void initGraphics(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_X, SCREEN_Y);
	glutCreateWindow("Quantum Demo");

	glutIdleFunc(idleFunc);
	glutDisplayFunc(drawFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutKeyboardFunc(keyFunc);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glPointSize(5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect = SCREEN_X / (float)SCREEN_Y;
	gluPerspective(45.0, aspect, 1.0, VIEWPORT_Z);
	glViewport(0, 0, SCREEN_X, SCREEN_Y);
	createDisplayListSphere();
	initLight();
}

// ************************** end GLUT methods

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	initGraphics(argc, argv);

	toggleScene(1);

	glutMainLoop();
	return 0;
}




