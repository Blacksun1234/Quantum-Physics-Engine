#include "stdafx.h"
#include <iostream>

#include "QmWorld.h"
#include "QmParticle.h"
#include "QmForceRegistery.h"
#include "QmForceGenerator.h"

using namespace Quantum;

QmWorld::QmWorld()
{
	std::cout << "Starting Quantum Physics engine." << std::endl;
	_time = 0.f;
	_ticktime = 0.f;
	gravity = glm::vec3(0, -9.81, 0);
	_delta = false;
}

QmWorld::~QmWorld()
{

}

void QmWorld::SetGravity(bool isGravityActive) {
	_isGravityActive = isGravityActive;
}

void QmWorld::SetDelta() {
	if (_delta) {
		_delta = false;
	}
	else {
		_delta = true;
	}
}

bool QmWorld::GetDelta() {
	return _delta;
}

float QmWorld::tickRK4(float t) {
	clear();// clear of particules to set acc to 0

	applyGravity(0);
	updateForces(0);
	integrate(t / 2.0f, 1);

	applyGravity(1);
	updateForces(1);
	integrate(t / 2.0f, 2);

	applyGravity(2);
	updateForces(2);
	integrate(t, 3);

	applyGravity(3);
	updateForces(3);
	integrate_RK4(t);

	_ticktime += t;
	return _time - _ticktime;
}

float QmWorld::tick(float t) {
	clear(); // clear of particules to set acc to 0
	applyGravity(0); //apply gravity to all bodies vector
	updateForces(0); // update forces to each iteration
	integrate(t,0);
	_ticktime += t;
	return _time - _ticktime;
}

void QmWorld::interpolate(float dt) {
	for (QmBody* b : bodies) {
		((QmParticle*)b)->GetUpdater()->update(((QmParticle*)b)->getPos(0) + dt * ((QmParticle*)b)->getVel(0));
	}
}

void QmWorld::simulate(float t)
{
	_time += t;
	float dt;
	bool useDELTA = _delta;
	float DELTA = 0.01f;
	dt = _time - _ticktime;
	if (useDELTA) { // deterministic framerate-independent simulation
		while (dt >= DELTA) {
			if (numericalIntegrator == 2) {
				dt = tickRK4(DELTA);
			}
			else {
				dt = tick(DELTA);
			}
			//printf("-\n");
		}
		//printf("------------\n");
		interpolate(dt);
	}
	else { // old fashioned all-frame non-deterministic simulation
		if (numericalIntegrator == 2) {
			tickRK4(t);
		}
		else {
			tick(t);
		}
		interpolate(0.0f);
	}
}

void QmWorld::integrate(float t, int i)
{
	if (numericalIntegrator == 0) {
		for (QmBody* b : bodies)
			b->integrateExplicit(t, i);
	}
	else if (numericalIntegrator == 1) {
		for (QmBody* b : bodies)
			b->integrateSemiExplicit(t, i);
	}
	else if (numericalIntegrator == 2) {
		for (QmBody* b : bodies)
			b->integrateRK4(t, i);
	}
}

void QmWorld::integrate_RK4(float t) {
	for (QmBody* b : bodies)
		b->integrate_RK4(t);
}

void QmWorld::applyGravity(int i) {
	if (_isGravityActive) {
		for (QmBody* b : bodies) {
			b->SetAcc(gravity, i);
		}	
	}
}

void QmWorld::updateForces(int i) {
	for (QmForceRegistery*  fr: forcesRegistries) {
		fr->fg->update(fr->p, i);
	}
}

void QmWorld::addBody(QmBody* b)
{
	bodies.push_back(b);
}

void QmWorld::addForceRegistery(QmForceRegistery* f) {
	forcesRegistries.push_back(f);
}

std::vector <QmForceRegistery*> QmWorld::getForcesRegistery() {
	return forcesRegistries;
}

std::vector<QmBody*> QmWorld::getBodies()
{
	return bodies;
}

int QmWorld::getNumericalIntegrator() {
	return numericalIntegrator;
}

void QmWorld::changeNumericalIntegrator() {
	numericalIntegrator++;
	numericalIntegrator %= 3;
}

bool Quantum::QmWorld::getGravityIsActive()
{
	return _isGravityActive;
}

void QmWorld::clear()
{

	for (QmBody* b : bodies)
	{
		b->clearParticle();
	}
}

void QmWorld::clearScene() {
	for (QmBody* b : bodies)
	{
		delete b;
	}
	for (QmForceRegistery* f : forcesRegistries)
	{
		delete f;
	}
	bodies.clear();
	forcesRegistries.clear();
}

