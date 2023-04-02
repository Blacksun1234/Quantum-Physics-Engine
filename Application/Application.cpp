// Physics-engine.cpp : définit le point d'entrée pour l'application console.
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
#include <sstream>

#include "glm/gtc/matrix_transform.hpp"
#include "Quantum.h"
#include "GxWorld.h"
#include "GxParticle.h"
#include "GxUpdater.h"
#include "QmForceRegistery.h"
#include "QmDrag.h"
#include "QmMagnetism.h"
#include "QmFixedMagnetism.h"
#include "QmSpring.h"
#include "QmFixedSpring.h"
#include "QmHalfspace.h"

using namespace std;
using namespace Quantum;

GxWorld gxWorld;
QmWorld pxWorld;

bool attract = true;

glm::vec3* mousePointer;

int scene = 1;

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
bool delta = true;

float restlength = 2.0f; // longueur câble au repos
float springConstant = 4000.0f; // puissance pr revenir au repos

GLfloat light_pos[] = { 0.0, 6.0, 1.0, 1.0 };

float cursormagnetisme = 0.0f;
// ********************************************

std::string text = "";

void updateText() {
	std::stringstream ss;
	ss << "Scene : " << (scene) << std::endl;
	ss << "Gravity: " << (pxWorld.getGravityIsActive() ? "True" : "False") << std::endl;
	ss << "Collision: " << (pxWorld.getCollisionIsActive() ? "True" : "False") << std::endl;
	ss << "BroadPhase: " << (pxWorld.getBroadPhaseIsActive() ? "True" : "False") << std::endl;
	if(scene == 2)
		ss << "Magnetism : " << cursormagnetisme << endl;
	ss << "Numerical Integrator: " << (pxWorld.getNumericalIntegrator()) << std::endl;
	ss << "Paused : " << (paused ? "True" : "False") << endl;
	ss << "Delta : " << (pxWorld.GetDelta() ? "True" : "False") << endl;
	ss << "restlength : " << restlength << endl;
	ss << "springConstant : "  << springConstant << endl;
	text = ss.str();
	glutPostRedisplay(); // request redisplay to show updated text
}

void renderText() {
	// Split the text string into separate lines based on newline characters
	std::vector<std::string> lines;
	std::istringstream ss(text);
	std::string line;
	while (std::getline(ss, line, '\n')) {
		lines.push_back(line);
	}

	// Render each line separately, with appropriate line spacing
	int lineHeight = 20; // adjust as needed
	int y = 20; // starting y position
	for (const auto& line : lines) {
		glRasterPos2f(10, y);
		for (unsigned int i = 0; i < line.length(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, line[i]);
		}
		y += lineHeight;
	}
}

glm::vec3 randomVector(float min, float max)
{
	float x = min + (max - min)*((rand() % 100) / 100.f);
	float y = min + (max - min)*((rand() % 100) / 100.f);
	float z = min + (max - min)*((rand() % 100) / 100.f);
	return glm::vec3(x, y, z);

}

QmParticle* createParticlePositive()
{
	glm::vec3 pos = randomVector(-5, 5);
	GxParticle* g = new GxParticle(glm::vec3(1,0,0), 0.5f , pos);
	QmParticle* p = new QmParticle(pos, randomVector(0, 0), randomVector(0, 0), 3, 10, 0.5f);
	p->setUpdater(new GxUpdater(g));
	gxWorld.addParticle(g);
	pxWorld.addBody(p);

	//add particules and link them in the forceRegistery in both ways

	for (QmBody* b : pxWorld.bodies) {
		if ((QmParticle*)b != p) {
			QmForceRegistery* f = new QmForceRegistery(p, new QmMagnetism(10.f, 5.0f,(QmParticle*)b ));
			pxWorld.addForceRegistery(f);
			QmForceRegistery* f2 = new QmForceRegistery( (QmParticle*)b, new QmMagnetism(10.f, 5.0f, p ));
			pxWorld.addForceRegistery(f2); 

			pxWorld.addForceRegistery(new QmForceRegistery((QmParticle*)b, new QmFixedMagnetism(10.f, 5.0f, mousePointer, &cursormagnetisme)));
		}
	}
	return p;
}

QmParticle* createParticleNegative()
{
	glm::vec3 pos = randomVector(-5, 5);
	GxParticle* g = new GxParticle(glm::vec3(0, 0, 1), 0.5f, pos);
	QmParticle* p = new QmParticle(pos, randomVector(0, 0), randomVector(0, 0), 3, -10, 0.5f);
	p->setUpdater(new GxUpdater(g));
	gxWorld.addParticle(g);
	pxWorld.addBody(p);

	//add particules and link them in the forceRegistery in both ways

	for (QmBody* b : pxWorld.bodies) {
		if ((QmParticle*)b != p) {
			QmForceRegistery* f = new QmForceRegistery(p, new QmMagnetism(10.f, 5.0f, (QmParticle*)b));
			pxWorld.addForceRegistery(f);
			QmForceRegistery* f2 = new QmForceRegistery((QmParticle*)b, new QmMagnetism(10.f, 5.0f, p));
			pxWorld.addForceRegistery(f2);
			pxWorld.addForceRegistery(new QmForceRegistery((QmParticle*)b, new QmFixedMagnetism(10.f, 5.0f, mousePointer, &cursormagnetisme)));
		}
	}
	return p;
}

QmParticle* createParticleFromCursorScene1()
{
	glm::vec3 pos = *mousePointer;
	GxParticle* g = new GxParticle(randomVector(1, 0), 0.5f, pos);
	QmParticle* p = new QmParticle(pos, randomVector(1, 10), randomVector(0, 0), 3, 10, 0.5f);
	QmForceRegistery* f = new QmForceRegistery(p, new QmDrag(1,2));
	p->setUpdater(new GxUpdater(g));
	gxWorld.addParticle(g);
	pxWorld.addBody(p);
	pxWorld.addForceRegistery(f);
	
	return p;
}

QmParticle* createParticleScene3(glm::vec3 pos)
{
	GxParticle* g = new GxParticle(randomVector(1, 0), 0.5f, pos);
	QmParticle* p = new QmParticle(pos, randomVector(1, 10), randomVector(0, 0), 3, 10, 0.5f);
	QmForceRegistery* f = new QmForceRegistery(p, new QmDrag(1, 2));
	p->setUpdater(new GxUpdater(g));
	gxWorld.addParticle(g);
	pxWorld.addBody(p);
	pxWorld.addForceRegistery(f);

	return p;
}

QmParticle* createParticleScene4(glm::vec3 pos, float mass, float radius)
{
	GxParticle* g = new GxParticle(randomVector(1, 0), radius, pos);
	QmParticle* p = new QmParticle(
		pos,
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		mass,
		0.0f,
		radius
	);

	p->setUpdater(new GxUpdater(g));
	gxWorld.addParticle(g);
	pxWorld.addBody(p);

	return p;
}

QmParticle* createParticleScene5()
{
	float radius = 0.1f + 0.2f * ((rand() % 100) / 100.f);
	glm::vec3 pos = randomVector(-4, 4);
	GxParticle* g = new GxParticle(randomVector(1, 0), radius, pos);
	QmParticle* p = new QmParticle(
		pos,
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		1 + rand() % 5,
		0.0f,
		radius
	);

	p->setUpdater(new GxUpdater(g));
	gxWorld.addParticle(g);
	pxWorld.addBody(p);

	return p;
}

QmHalfspace* createHalfspace(glm::vec3 normal, float offset) {
	QmHalfspace* h = new QmHalfspace(normal, offset);
	pxWorld.addBody(h);
	return h;
}

void createMagnetismeFromCursorScene2()
{
	glm::vec3 pos = *mousePointer;
	
	if (cursormagnetisme == 20.f) {
		for (QmBody* b : pxWorld.bodies) {
			cursormagnetisme = 0.f;
			pxWorld.addForceRegistery(new QmForceRegistery((QmParticle*)b, new QmFixedMagnetism(10.f, 5.0f, mousePointer, &cursormagnetisme)));
		}
	}
	if (attract) {
		cursormagnetisme = 500.f;
	}
	else {
		cursormagnetisme = -500.f;
	}
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
	for (int i = 0; i < 30; i++) {
		createParticlePositive();
		createParticleNegative();
	}
}

void initScene3() {
	printf("Scene 3.\n");
	mousePointer = new glm::vec3(0, 4.5, 0);
	QmParticle* p1 = createParticleScene3(glm::vec3(-1, 0, -1));
	QmParticle* p2 = createParticleScene3(glm::vec3(1, 0, -1));
	QmParticle* p3 = createParticleScene3(glm::vec3(0, 0, 1));

	QmParticle* p4 = createParticleScene3(glm::vec3(0, -1, 0));
	QmParticle* p5 = createParticleScene3(glm::vec3(0, -2, 0));

	QmParticle* p6 = createParticleScene3(glm::vec3(-1, -3, -1));
	QmParticle* p7 = createParticleScene3(glm::vec3(1, -3, -1));
	QmParticle* p8 = createParticleScene3(glm::vec3(0, -3, 1));

	QmParticle* p9 = createParticleScene3(glm::vec3(0, -4, 0));

	QmParticle* p10 = createParticleScene3(glm::vec3(-1, -5, -1));
	QmParticle* p11 = createParticleScene3(glm::vec3(1, -5, -1));
	QmParticle* p12 = createParticleScene3(glm::vec3(0, -5, 1));

	pxWorld.addForceRegistery(new QmForceRegistery(p1, new QmFixedSpring(mousePointer, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p2, new QmFixedSpring(mousePointer, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p3, new QmFixedSpring(mousePointer, &restlength, &springConstant)));

	pxWorld.addForceRegistery(new QmForceRegistery(p1, new QmSpring(p2, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p1, new QmSpring(p3, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p2, new QmSpring(p1, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p2, new QmSpring(p3, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p3, new QmSpring(p1, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p3, new QmSpring(p2, &restlength, &springConstant)));

	pxWorld.addForceRegistery(new QmForceRegistery(p4, new QmSpring(p1, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p4, new QmSpring(p2, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p4, new QmSpring(p3, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p1, new QmSpring(p4, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p2, new QmSpring(p4, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p3, new QmSpring(p4, &restlength, &springConstant)));

	pxWorld.addForceRegistery(new QmForceRegistery(p4, new QmSpring(p5, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p5, new QmSpring(p4, &restlength, &springConstant)));

	pxWorld.addForceRegistery(new QmForceRegistery(p5, new QmSpring(p6, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p5, new QmSpring(p7, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p5, new QmSpring(p8, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p6, new QmSpring(p5, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p7, new QmSpring(p5, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p8, new QmSpring(p5, &restlength, &springConstant)));

	pxWorld.addForceRegistery(new QmForceRegistery(p6, new QmSpring(p7, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p6, new QmSpring(p8, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p7, new QmSpring(p6, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p7, new QmSpring(p8, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p8, new QmSpring(p6, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p8, new QmSpring(p7, &restlength, &springConstant)));

	pxWorld.addForceRegistery(new QmForceRegistery(p8, new QmSpring(p9, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p9, new QmSpring(p8, &restlength, &springConstant)));

	pxWorld.addForceRegistery(new QmForceRegistery(p9, new QmSpring(p10, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p9, new QmSpring(p11, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p9, new QmSpring(p12, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p10, new QmSpring(p9, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p11, new QmSpring(p9, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p12, new QmSpring(p9, &restlength, &springConstant)));

	pxWorld.addForceRegistery(new QmForceRegistery(p10, new QmSpring(p11, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p10, new QmSpring(p12, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p11, new QmSpring(p10, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p11, new QmSpring(p12, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p12, new QmSpring(p10, &restlength, &springConstant)));
	pxWorld.addForceRegistery(new QmForceRegistery(p12, new QmSpring(p11, &restlength, &springConstant)));

}

void initScene4() {
	mousePointer = new glm::vec3(0, 4.5, 0);
	createParticleScene4(glm::vec3(-3, 0, 0), 10, 1);
	createParticleScene4(glm::vec3(-3, 2, 0), 1, 0.1);
	createParticleScene4(glm::vec3(3, 0, 0), 1, 0.1);
	createParticleScene4(glm::vec3(3, 2, 0), 10, 1);
	createHalfspace(glm::vec3(0, 1, 0), -2);
}

void initScene5() {
	mousePointer = new glm::vec3(0, 0, 0);
	for (int i = 0; i < 200; i++)
		createParticleScene5();
	createHalfspace(glm::vec3(1, 0, 0), -5);
	createHalfspace(glm::vec3(-1, 0, 0), -5);
	createHalfspace(glm::vec3(0, 1, 0), -5);
	createHalfspace(glm::vec3(0, -1, 0), -5);
	createHalfspace(glm::vec3(0, 0, 1), -5);
	createHalfspace(glm::vec3(0, 0, -1), -5);
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
		if (scene == 1)
			createParticleFromCursorScene1();
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

	if (scene == 3) {
		for (QmForceRegistery* qmFR : pxWorld.getForcesRegistery()) {
			QmForceGenerator* qmFG = qmFR->fg;

			if (dynamic_cast<QmSpring*> (qmFG)) {
				glBegin(GL_LINES);
				glVertex3f(qmFR->p->getPos(0).x, qmFR->p->getPos(0).y, qmFR->p->getPos(0).z);
				glVertex3f(((QmSpring*)qmFG)->getOtherParticle()->getPos(0).x, ((QmSpring*)qmFG)->getOtherParticle()->getPos(0).y, ((QmSpring*)qmFG)->getOtherParticle()->getPos(0).z);
				glEnd();
			}

			else if (dynamic_cast<QmFixedSpring*> (qmFG)) {
				glBegin(GL_LINES);
				glVertex3f(qmFR->p->getPos(0).x, qmFR->p->getPos(0).y, qmFR->p->getPos(0).z);
				glVertex3f(((QmFixedSpring*)qmFG)->getFixedPos()->x, ((QmFixedSpring*)qmFG)->getFixedPos()->y, ((QmFixedSpring*)qmFG)->getFixedPos()->z);
				glEnd();
			}
		}
	}

	for (QmBody* b : pxWorld.getBodies()) {
		if (dynamic_cast<QmHalfspace*>(b)) {
			QmHalfspace* h = (QmHalfspace*)b;
			glm::vec3 n = glm::normalize(h->normal);
			float o = h->offset;
			float ox = n.x * o;
			float oy = n.y * o;
			float oz = n.z * o;

			glBegin(GL_LINES);
			glm::vec3 v1(1.0f, 0.0f, 0.0f);
			if (glm::length(n - v1) < 0.001f || glm::length(n + v1) < 0.001f)
				v1 = glm::vec3(0.0f, 1.0f, 0.0f);

			glm::vec3 v2 = glm::normalize(glm::cross(n, v1)) * 5.0f;
			glm::vec3 v3 = glm::normalize(glm::cross(n, v2)) * 5.0f;

			glVertex3f(v2.x + v3.x + ox, v2.y + v3.y + oy, v2.z + v3.z + oz);
			glVertex3f(-v2.x + v3.x + ox, -v2.y + v3.y + oy, -v2.z + v3.z + oz);

			glVertex3f(-v2.x + v3.x + ox, -v2.y + v3.y + oy, -v2.z + v3.z + oz);
			glVertex3f(-v2.x - v3.x + ox, -v2.y - v3.y + oy, -v2.z - v3.z + oz);

			glVertex3f(-v2.x - v3.x + ox, -v2.y - v3.y + oy, -v2.z - v3.z + oz);
			glVertex3f(v2.x - v3.x + ox, v2.y - v3.y + oy, v2.z - v3.z + oz);

			glVertex3f(v2.x - v3.x + ox, v2.y - v3.y + oy, v2.z - v3.z + oz);
			glVertex3f(v2.x + v3.x + ox, v2.y + v3.y + oy, v2.z + v3.z + oz);

			glEnd();
		}
	}

	// save the current projection matrix and set up an orthographic projection
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, SCREEN_X, SCREEN_Y, 0, -1, 1);

	// reset the modelview matrix to the identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	updateText();

	// draw the text at a fixed position on the screen
	renderText();

	// restore the original projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glutSwapBuffers();
}


void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) buttons = 1;
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) buttons = 2;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) buttons = 3;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) cursormagnetisme = 20.0f;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) cursormagnetisme = 0.0f;

	if (button == 3) camDist /= 1.07f;
	if (button == 4) camDist *= 1.07f;
	if (state == GLUT_UP) buttons = 0;

	mx = (float)x;
	my = (float)y;

	//updateText();
}

void motionFunc(int x, int y)
{
	if (buttons == 1)
	{
		camRotate += (x - mx) / 100.f;
		camHeight += (y - my) * camDist / 1000.f;
		if (scene == 5) {
			for (QmBody* b : pxWorld.getBodies()) {
				if (dynamic_cast<QmParticle*>(b)) {
					float v = (y - my) * camDist / 1000.0f;
					((QmParticle*)b)->SetPos(glm::vec3(0.0f, v, 0.0f), 0);
				}
			}
		}
	}
	if (buttons == 3)
	{
		if (scene == 5) {
			for (QmBody* b : pxWorld.getBodies()) {
				if (dynamic_cast<QmHalfspace*>(b)) {
					float xRotationAngle = glm::radians(x - mx);
					float yRotationAngle = glm::radians(my - y);
					glm::mat4 xRotationMatrix = glm::rotate(glm::mat4(1.0f), xRotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
					glm::mat4 yRotationMatrix = glm::rotate(glm::mat4(1.0f), yRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
					glm::mat4 transformationMatrix = yRotationMatrix * xRotationMatrix * glm::mat4(1.0f);
					glm::vec4 vectorInFirstWorldHomogeneous(((QmHalfspace*)b)->normal, 1.0f);
					glm::vec4 vectorInSecondWorldHomogeneous = transformationMatrix * vectorInFirstWorldHomogeneous;
					((QmHalfspace*)b)->normal = glm::vec3(vectorInSecondWorldHomogeneous) / vectorInSecondWorldHomogeneous.w;
				}
			}
		}
		if (mousePointer)
			*mousePointer += glm::vec3(x - mx, my - y, 0.f) / 15.f;
	}

	mx = (float)x;
	my = (float)y;
}

void clearWorld()
{
	gxWorld.clear();
	pxWorld.clearScene();

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
	case 3: initScene3(); break;
	case 4: initScene4(); break;
	case 5: initScene5(); break;
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
		toggleScene(1);
		break;
	case '2':
		toggleScene(2);
		break;
	case '3':
		toggleScene(3);
		break;
	case '4':
		toggleScene(4);
		break;
	case '5':
		toggleScene(5);
		break;
	case ' ':
		paused = !paused;
		break;
	case 'g':
		pxWorld.SetGravity(!pxWorld.getGravityIsActive());
		cout << "Gravity : " << pxWorld.getGravityIsActive() << endl;
		break;
	case 'f':
		if (attract) {
			attract = false;
		}
		else {
			attract = true;
		}
		break;
	case 'd':
		pxWorld.SetDelta();
		break;
	case 's':
		pxWorld.changeNumericalIntegrator();
		cout << "Numerical Integrator:" << pxWorld.getNumericalIntegrator() << endl;
		break;
	case 'o':
		restlength += 100;
		break;
	case 'l':
		restlength -= 100;
		break;
	case 'p':
		springConstant += 100;
		break;
	case 'm':
		springConstant -= 100;
		break;
	case 'k':
		cout << pxWorld.getForcesRegistery().size() << endl;
		break;
	case 'c':
		pxWorld.SetCollision(!pxWorld.getCollisionIsActive());
		cout << "Collision : " << pxWorld.getCollisionIsActive() << endl;
		break;
	case 'b':
		pxWorld.SetBroadPhase(!pxWorld.getBroadPhaseIsActive());
		cout << "BroadPhase : " << pxWorld.getBroadPhaseIsActive() << endl;
		break;
	default:
		break;
	}
	updateText();
}

void reshapePort(int width, int height) {
	SCREEN_X = width;
	SCREEN_Y = height;
	float aspect = SCREEN_X / (double)SCREEN_Y;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500.0);

	glutPostRedisplay();
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
	glutReshapeFunc(reshapePort);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
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
	updateText();

	glutMainLoop();
	return 0;
}
