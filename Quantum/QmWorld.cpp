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
	//isGravityActive = true;
	gravity = glm::vec3(0, -9.81, 0);
	_delta = false;
}

QmWorld::~QmWorld()
{

}

void QmWorld::SetGravity(bool isGravityActive) {
	_isGravityActive = isGravityActive;
}

void QmWorld::SetDelta(bool delta) {
	_delta = delta;
}

bool QmWorld::GetDelta() {
	return _delta;
}

float QmWorld::tick(float t) {
	clear(); // clear of particules to set acc to 0
	applyGravity(); //apply gravity to all bodies vector
	updateForces(); // update forces to each iteration
	integrate(t);
	_ticktime += t;
	return _time - _ticktime;
}

void QmWorld::interpolate(float dt) {
	for (QmBody* b : bodies) {
		((QmParticle*)b)->GetUpdater()->update(((QmParticle*)b)->getPos() + dt * ((QmParticle*)b)->getVel());
	}
}

void QmWorld::simulate(float t)
{
	_time += t;
	float dt;
	bool useDELTA = _delta;
	float DELTA = 0.1f;
	dt = _time - _ticktime;
	if (useDELTA) { // deterministic framerate-independent simulation
		while (dt >= DELTA) {
			dt = tick(DELTA);
			printf("-\n");
		}
		printf("------------\n");
		interpolate(dt);
	}
	else { // old fashioned all-frame non-deterministic simulation
		tick(t);
		interpolate(0.0f);
	}
}

void QmWorld::integrate(float t)
{
	for (QmBody* b : bodies)
		b->integrate(t);
}

void QmWorld::applyGravity() {
	if (_isGravityActive) {
		for (QmBody* b : bodies) {
			b->SetAcc(gravity);
		}	
	}
}

void QmWorld::updateForces() {
	for (QmForceRegistery*  fr: forcesRegistries) {
		fr->fg->update(fr->p);
	}
}

void QmWorld::addBody(QmBody* b)
{
	bodies.push_back(b);
}

void QmWorld::addForceRegistery(QmForceRegistery* f) {
	forcesRegistries.push_back(f);
}

std::vector<QmBody*> QmWorld::getBodies()
{
	return bodies;
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

